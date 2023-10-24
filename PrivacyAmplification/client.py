import pika
import math
import numpy as np
import time


# Notation Definition
# T Negotiation key
# n Length of negotiation key
# TMi The i-th group negotiation key
# M Group length of negotiation key
# K Number of groups negotiating key
# Nj The j-th block negotiation key
# N Length of Cellular Automata
# C Reciprocal of key compression rate and number of blocks
# Hi Final key obtained by group i
# H Final key
# m Length of zero complement in negotiation key T


def privacy_amplification(T, K, M, N, initial_value):
    if len(T) > M * K:
        print("M*K cannot be less than key lenght")
        exit(-1)
    # Step 1: Divide the received n bits negotiation key T into small groups with length M
    if len(T) < M * K:
        # Add zeros to the last group if necessary
        padded_key = np.pad(T, (0, (M * K) - len(T)), mode='constant')
        T_groups = np.array_split(padded_key, K)
    else:
        T_groups = np.array_split(T, K)

    # Step 2: Initialize CA and set its running rules
    CA = np.zeros((N,), dtype=int)
    CA[:N] = initial_value
    accumulator = np.zeros((N,), dtype=int)
    # Step 3: Compress each group negotiation key
    final_key = np.zeros((K * N,), dtype=int)
    for i, T_group in reversed(list(enumerate(T_groups))):
        T_blocks = np.array_split(T_group, math.ceil(M / N))
        for T_block in T_blocks:
            print( "CA before update: "+ str(int("".join([str(x) for x in CA]), 2)))
            ands = np.bitwise_and(T_block, CA)
            print("Ands: "+ str(int("".join([str(x) for x in ands]), 2)))
            CA = update(CA)
            print("CA after update: "+ str(int("".join([str(x) for x in CA]), 2)))
            accumulator = np.bitwise_xor(accumulator, ands)
            print("Accumulator: "+str(int("".join([str(x) for x in accumulator]), 2)))
        final_key[i * N:(i + 1) * N] = accumulator
        CA = accumulator
        CA=update(CA)
        print("End group\n\n")
    # Step 4: Combine the final key blocks into a final security key
    return final_key


def rule(left_cell, center_cell, right_cell):
    rule_number = 150
    ruleN = format(rule_number, '08b')
    return int(ruleN[(left_cell << 2 | center_cell << 1 | right_cell)])


def update(CA):
    new = np.empty(len(CA), dtype=int)
    for i in range(len(CA)):
        if (i == len(CA) - 1):
            new[i] = rule(CA[i - 1], CA[i], CA[0])
            continue
        if (i == 0):
            new[i] = rule(CA[len(CA) - 1], CA[i], CA[i + 1])
            continue
        new[i] = rule(CA[i - 1], CA[i], CA[i + 1])
    return new

def start_client(key, port, usr, psw, host, M, N, lenght,seq):
    """
    Main function that sets up the RabbitMQ connection and starts the consumer.
    """
    seed = np.random.randint(2, size=N)
    intKey = [int(x, 2) for x in key]
    credentials = pika.PlainCredentials(usr, psw)
    connection = pika.BlockingConnection(pika.ConnectionParameters(host=host, port=port, credentials=credentials))
    channel = connection.channel()
    K = math.ceil(lenght / M)
    start_time = time.time()
    newKey = privacy_amplification(intKey, K, M, N, seed)
    channel.queue_declare(queue="clientQueuePA"+str(seq))
    channel.queue_declare(queue="serverQueuePA"+str(seq))
    channel.basic_publish(exchange='',
                          routing_key='serverQueuePA'+str(seq),
                          properties=pika.BasicProperties(
                              reply_to='clientQueuePA'+str(seq),
                              headers={'K': K, 'M': M, 'N': N, 'seed': "".join([str(x) for x in seed])}
                              # Add a key/value header
                          ),
                          body=b'init PA')
    for method_frame, properties, body in channel.consume(queue='clientQueuePA'+str(seq), auto_ack=True):
        if body != b'PA Completed':
            print("Error in PA RPC")
            exit(-1)
        break
    elapsed_time = time.time() - start_time
    channel.close()
    connection.close()
    return elapsed_time
