#include "FastaFstBuilder.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <thread>
#include <mutex>

std::mutex builder_mutex; // Mutex for synchronizing access to the builder

// Function to process a chunk of data
void ProcessSequenceChunk(std::vector<std::string> &sequences, fst::FastaFstBuilder &builder){
  for (const auto &seq : sequences)
  {
    builder.AddSequence(seq);
  }
}

// Main processing function for FASTA format
void ProcessFasta(const std::string &input_file, const std::string &output_file){
  std::ifstream fin(input_file);
  if (!fin.is_open()){
    throw std::runtime_error("Cannot open input file: " + input_file);
  }

  fst::FastaFstBuilder builder;
  std::string line, sequence;
  std::vector<std::string> sequences_batch; // Buffer for sequences

  while (std::getline(fin, line)){
    if (line.empty())
      continue;

    // Handling FASTA format
    if (line[0] == '>'){
      // When a new sequence is encountered, process the previous sequence if it exists
      if (!sequence.empty()){
        sequences_batch.push_back(sequence);
        sequence.clear();
      }
    }
    else{
      // Append the sequence line
      sequence += line;
    }

    // Process sequences in batches when batch size is met
    if (sequences_batch.size() >= 1000){
      std::vector<std::string> batch_to_process = std::move(sequences_batch);
      std::thread processing_thread(ProcessSequenceChunk, std::ref(batch_to_process), std::ref(builder));
      processing_thread.join(); // Wait for processing to finish
    }
  }

  // Process any remaining sequences
  if (!sequence.empty()){
    sequences_batch.push_back(sequence);
  }

  if (!sequences_batch.empty()){
    std::vector<std::string> batch_to_process = std::move(sequences_batch);
    std::thread processing_thread(ProcessSequenceChunk, std::ref(batch_to_process), std::ref(builder));
    processing_thread.join(); // Wait for processing to finish
  }

  // Save the final result
  builder.Save(output_file);
}

int main(int argc, char **argv){
  if (argc != 3){
    std::cerr << "Usage: " << argv[0] << " <input.fasta> <output.fst>\n";
    return 1;
  }

  try{
    ProcessFasta(argv[1], argv[2]);
    std::cout << "Successfully converted " << argv[1]
              << " to " << argv[2] << std::endl;
  }
  catch (const std::exception &e){
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
