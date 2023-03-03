import pika
import math
import numpy as np
def privacy_amplification(T, K, M, N, initial_value):
    if len(T)>M*K:
        print("M*K cannot be less than key lenght")
        exit(-1)
    # Step 1: Divide the received n bits negotiation key T into small groups with length M
    if len(T) < M*K:
        # Add zeros to the last group if necessary
        padded_key=np.pad(T, (0, (M*K)-len(T)), mode='constant')
        T_groups = np.array_split(padded_key, K)
    else:
        T_groups = np.array_split(T, K)

    # Step 2: Initialize CA and set its running rules
    CA = np.zeros((N,), dtype=int)
    CA[:N] = initial_value
    accumulator=np.zeros((N,), dtype=int)
    # Step 3: Compress each group negotiation key
    final_key = np.zeros((K * N,), dtype=int)
    for i, T_group in enumerate(T_groups):
        T_blocks = np.array_split(T_group, math.ceil(M/N))
        for T_block in T_blocks:
            ands = np.bitwise_and(T_block, CA)
            CA = update(CA)
            accumulator = np.bitwise_xor(accumulator, ands)
        final_key[i*N:(i+1)*N] = accumulator
        CA = final_key[i*N:(i+1)*N]

    # Step 4: Combine the final key blocks into a final security key
    return final_key

def rule(left_cell, center_cell, right_cell):
    rule_number = 150
    ruleN = format(rule_number, '08b')
    return int(ruleN[7 - (left_cell << 2) - (center_cell << 1) - right_cell])


def update(CA):
    new=np.empty(len(CA), dtype=int)
    for i in range(len(CA)):
        if(i==len(CA)-1):
            new[i] = rule(CA[i - 1], CA[i], CA[0])
            continue
        if(i==0):
            new[i] = rule(CA[len(CA)-1], CA[i], CA[i+1])
            continue
        new[i]=rule(CA[i-1],CA[i],CA[i+1])
    return new




  #   ch.basic_publish(exchange='',
  #                   routing_key=props.reply_to,
  #                   properties=pika.BasicProperties(correlation_id = \
   #                                                  props.correlation_id),
   #                  body=response)
    #ch.basic_ack(delivery_tag = method.delivery_tag)

def start_server(key):
     connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost'))
     channel = connection.channel()
     intKey = [int (x,2)  for x in key]
     channel.queue_declare(queue='serverQueue')
     # Get ten messages and break out
     for method_frame, properties, body in channel.consume(queue='serverQueue'):

        on_request(method_frame,properties,body,intKey)
        # Acknowledge the message
        channel.basic_ack(method_frame.delivery_tag)
        break

def on_request(method, props, body,key):
 #   key=[int (x,2)  for x in props.headers['key'][1:len(props.headers['key'])-1].split()]
    K=int(props.headers['K'])
    M=int(props.headers['M'])
    N=int(props.headers['N'])
    seed=[int (x,2)  for x in props.headers['seed'][1:len(props.headers['seed'])-1].split()]


    response = privacy_amplification(key, K, M,N,seed)
    print('Server sifted key: ', response, 'len:', len(response))
