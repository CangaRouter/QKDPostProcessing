#include "Client.h"
#include "algorithm.h"
#include "debug.h"
#include "classical_session.h"
#include <cassert>
#include <cmath>
#include <ctime>
#include <boost/algorithm/string.hpp>

using namespace Cascade;

Client::Client(const Algorithm &algorithm, ClassicalSession &classicalSession, int nr_bits, double noise) : algorithm(
        algorithm),
                                                                                                            classical_session(
                                                                                                                    classicalSession),
                                                                                                            reconciled_key(
                                                                                                                    classical_session.initialization()),
                                                                                                            nr_key_bits(
                                                                                                                    nr_bits),
                                                                                                            estimated_bit_error_rate(
                                                                                                                    noise),noisyKey(reconciled_key){

    stats.keySize = nr_bits;
}


Client::~Client() {

}

const Algorithm &Client::get_algorithm() const {
    return algorithm;
}

double Client::get_estimated_bit_error_rate() const {
    return estimated_bit_error_rate;
}

Key &Client::get_reconciled_key() {
    return reconciled_key;
}


int Client::get_nr_key_bits() const {
    return nr_key_bits;
}

Stats &Client::get_stats() {
    return stats;
}

static double elapsed_time(const struct timespec &start, const struct timespec &end) {
    double d_start = double(start.tv_sec) + double(start.tv_nsec) / 1000000000.0;
    double d_end = double(end.tv_sec) + double(end.tv_nsec) / 1000000000.0;
    return d_end - d_start;
}

double Client::compute_efficiency(long reconciliation_bits) const {
    double eps = estimated_bit_error_rate;
    double shannon_efficiency = -eps * log2(eps) - (1.0 - eps) * log2(1.0 - eps);
    int key_size = reconciled_key.get_nr_bits();
    double efficiency = double(reconciliation_bits) / (double(key_size) * shannon_efficiency);
    return efficiency;
}

void Client::reconcile() {
    // Record start time.
    struct timespec start_process_time;
    int rc = clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start_process_time);
    assert(rc == 0);
    struct timespec start_real_time;
    rc = clock_gettime(CLOCK_MONOTONIC, &start_real_time);
    assert(rc == 0);

    // Normal cascade iterations.
    all_normal_cascade_iterations();

    // BICONF iterations (if any).
    all_biconf_iterations();

    // Record end time.
    struct timespec end_process_time;
    rc = clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end_process_time);
    assert(rc == 0);
    struct timespec end_real_time;
    rc = clock_gettime(CLOCK_MONOTONIC, &end_real_time);
    assert(rc == 0);

    // Compute elapsed time.
    stats.elapsed_process_time = elapsed_time(start_process_time, end_process_time);
    stats.elapsed_real_time = elapsed_time(start_real_time, end_real_time);

    // Compute efficiencies.
    stats.unrealistic_efficiency = compute_efficiency(stats.ask_parity_blocks);
    long reconciliation_bits = stats.start_iteration_bits + stats.ask_parity_bits +
                               stats.reply_parity_bits;
    stats.realistic_efficiency = compute_efficiency(reconciliation_bits);
    Key correctKey = classical_session.closeConnection();
    stats.setActualBitErrors(correctKey.nr_bits_different(noisyKey));
    stats.setRemainingBitErrors(correctKey.nr_bits_different(reconciled_key));
    stats.setCorrectKey(correctKey);
}

void Client::all_normal_cascade_iterations() {
    int iteration_nr = 0;
    for (int i = 0; i < algorithm.nr_cascade_iterations; ++i) {
        ++stats.normal_iterations;
        ++iteration_nr;
        start_iteration_common(iteration_nr, false);
        service_all_pending_work(true);
    }
}

void Client::all_biconf_iterations() {
    if (algorithm.nr_biconf_iterations == 0)
        return;
    int iterations_to_go = algorithm.nr_biconf_iterations;
    int iteration_nr = algorithm.nr_cascade_iterations;
    while (iterations_to_go > 0) {
        ++stats.biconf_iterations;
        ++iteration_nr;
        start_iteration_common(iteration_nr, true);
        int errors_corrected = service_all_pending_work(algorithm.biconf_cascade);
        if (algorithm.biconf_error_free_streak and errors_corrected > 0)
            iterations_to_go = algorithm.nr_biconf_iterations;
        else
            iterations_to_go -= 1;
    }
}

void Client::start_iteration_common(int iteration_nr, bool biconf) {

    IterationPtr iteration(new Iteration(*this, iteration_nr, biconf));
    iterations.push_back(iteration);
    stats.start_iteration_messages += 1;
    classical_session.start_iteration_with_shuffle_seed(iteration_nr,
                                                        iteration->get_shuffle()->get_seed());
    stats.start_iteration_bits += 32 + 64;

    iteration->schedule_initial_work();
}

void Client::schedule_try_correct(BlockPtr block, bool correct_right_sibling) {
    PendingItem pending_item(block, correct_right_sibling);
    pending_try_correct_blocks.push_back(pending_item);
}

void Client::schedule_ask_correct_parity(BlockPtr block, bool correct_right_sibling) {
    stats.ask_parity_bits += block->encoded_bits();
    PendingItem pending_item(block, correct_right_sibling);
    pending_ask_correct_parity_blocks.push_back(pending_item);

}

void Client::correct_orig_key_bit(int orig_key_bit_nr, int triggering_iteration_nr,
                                  bool cascade) {
    reconciled_key.flip_bit(orig_key_bit_nr);
    for (IterationPtr iteration: iterations)
        iteration->flip_parity_in_all_blocks_containing_bit(orig_key_bit_nr);
    if (cascade)
        cascade_effect(orig_key_bit_nr, triggering_iteration_nr);
}

void Client::cascade_effect(int orig_key_bit_nr, int triggering_iteration_nr) {
    // Re-visit every cascade iteration up to now, except the one that triggered this cascade.
    for (IterationPtr iteration: iterations) {
        if (iteration->get_iteration_nr() != triggering_iteration_nr) {
            // Each iteration can contribute at most one block to cascade into. If there is
            // such a block, schedule it for a try-correct.
            BlockPtr block(iteration->get_cascade_block(orig_key_bit_nr));
            if (block) {
                schedule_try_correct(block, false);
            }
        }
    }
}

int Client::service_all_pending_work(bool cascade) {
    int errors_corrected = 0;
    while (!pending_ask_correct_parity_blocks.empty() || !pending_try_correct_blocks.empty()) {
        errors_corrected += service_pending_try_correct(cascade);
        service_pending_ask_correct_parity();
    }
    return errors_corrected;
}

int Client::service_pending_try_correct(bool cascade) {
    int errors_corrected = 0;
    while (!pending_try_correct_blocks.empty()) {
        PendingItem pending_item = pending_try_correct_blocks.front();
        pending_try_correct_blocks.pop_front();
        Iteration &iteration = pending_item.block->get_iteration();
        errors_corrected += iteration.try_correct_block(pending_item.block,
                                                        pending_item.correct_right_sibling,
                                                        cascade);
    }
    return errors_corrected;
}

void Client::ask_correct_parities(PendingItemQueue &ask_correct_parity_blocks) {
    // Once we implement the real classical session, we will need to keep track of the blocks
    // for which we asked Alice the correct parity, but for which we have not yet received the
    // answer from Alice. For now, assume we get the answer immediately.
    int *iteration_nr = new int[ask_correct_parity_blocks.size()];
    int *start_bit = new int[ask_correct_parity_blocks.size()];
    int *end_bit = new int[ask_correct_parity_blocks.size()];
    int *correct_parity;
    int i = 0;
    for (auto it = ask_correct_parity_blocks.begin(); it != ask_correct_parity_blocks.end(); ++it) {
        PendingItem pending_item(*it);
        BlockPtr block = pending_item.block;
        iteration_nr[i] = block->get_iteration().get_iteration_nr();
        start_bit[i] = block->get_start_bit_nr();
        end_bit[i] = block->get_end_bit_nr();
        ++i;
    }
    i = 0;
    correct_parity = classical_session.channel_correct_parities(iteration_nr, start_bit, end_bit,
                                                                ask_correct_parity_blocks.size());
    for (auto it = ask_correct_parity_blocks.begin(); it != ask_correct_parity_blocks.end(); ++it) {
        PendingItem pending_item(*it);
        BlockPtr block = pending_item.block;
        block->set_correct_parity(*correct_parity);
        correct_parity++;
    }
}

void Client::service_pending_ask_correct_parity() {
    // Ask Alice for the correct parity for each block on the ask-parity list.
    stats.ask_parity_messages += 1;
    stats.ask_parity_blocks += pending_ask_correct_parity_blocks.size();
    stats.reply_parity_bits += pending_ask_correct_parity_blocks.size();
    if (!pending_ask_correct_parity_blocks.empty())
        ask_correct_parities(pending_ask_correct_parity_blocks);
    // Move all blocks over to the try-correct list.
    while (!pending_ask_correct_parity_blocks.empty()) {
        PendingItem pending_item = pending_ask_correct_parity_blocks.front();
        pending_ask_correct_parity_blocks.pop_front();
        schedule_try_correct(pending_item.block, pending_item.correct_right_sibling);
    }

}
