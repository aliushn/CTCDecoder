#include "ctc_greedy_decoder.h"
#include "decoder_utils.h"

std::pair<std::string,std::string> ctc_greedy_decoder(
    const std::vector<std::vector<double>> &probs_seq,
    const std::vector<std::string> &vocabulary) {
  // dimension check
  size_t num_time_steps = probs_seq.size();
  for (size_t i = 0; i < num_time_steps; ++i) {
    VALID_CHECK_EQ(probs_seq[i].size(),
                   vocabulary.size() + 1,
                   "The shape of probs_seq does not match with "
                   "the shape of the vocabulary");
  }

  size_t blank_id = vocabulary.size();

  std::vector<size_t> max_idx_vec(num_time_steps, 0);
  std::vector<size_t> idx_vec;
  std::vector<int> space_idx;
  for (size_t i = 0; i < num_time_steps; ++i) {
    double max_prob = 0.0;
    size_t max_idx = 0;
    const std::vector<double> &probs_step = probs_seq[i];
    for (size_t j = 0; j < probs_step.size(); ++j) {
      if (max_prob < probs_step[j]) {
        max_idx = j;
        max_prob = probs_step[j];
      }
    }
    // id with maximum probability in current time step
    max_idx_vec[i] = max_idx;
    // deduplicate
    if ((i == 0) || ((i > 0) && max_idx_vec[i] != max_idx_vec[i - 1])) {
      idx_vec.push_back(max_idx_vec[i]);
      if (vocabulary[max_idx_vec[i]]==" ")
        space_idx.push_back(i);
  
    }
  }

  std::string best_path_result;
  std::string spaces;
  int j = 0;
  for (size_t i = 0; i < idx_vec.size(); ++i) {
    if (idx_vec[i] != blank_id) {
      best_path_result += vocabulary[idx_vec[i]];
      if (vocabulary[idx_vec[i]]==" ")
        spaces += std::to_string(space_idx[j])+"#";
        j++;
    }
  }
  std::pair<std::string, std::string> result(spaces, best_path_result);
  return result;
}
