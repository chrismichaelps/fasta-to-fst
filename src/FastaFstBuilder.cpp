#include "FastaFstBuilder.hpp"
#include <cctype>
#include <stdexcept>
#include <sys/stat.h>
#include <sys/types.h>

namespace fst{

  FastaFstBuilder::FastaFstBuilder() : syms("amino_acids"){
    // Initialize the symbol table and initial state of the FST.
    InitializeSymbolTable();
    fsa.AddState();
    fsa.SetStart(0);
  }

  void FastaFstBuilder::InitializeSymbolTable(){
    // The epsilon symbol is added at position 0.
    syms.AddSymbol("<eps>", 0);
    // The 20 standard amino acids are defined.
    const std::string symbols = "ACDEFGHIKLMNPQRSTVWY";
    for (size_t i = 0; i < symbols.size(); ++i){
      syms.AddSymbol(std::string(1, symbols[i]), i + 1);
    }
  }

  int FastaFstBuilder::GetSymbolId(char c) const{
    // Convert the character to uppercase and look up the ID in the symbol table.
    char upper = std::toupper(c);
    std::string symbol(1, upper);
    int id = syms.Find(symbol);
    return id; // If not found, returns -1.
  }

  void FastaFstBuilder::AddSequence(const std::string &sequence){
    int current_state = fsa.Start();
    // Loop through each character in the sequence.
    for (char c : sequence){
      int symbol_id = GetSymbolId(c);
      // If the symbol is invalid (for example, spaces or other characters), it is ignored.
      if (symbol_id == -1)
        continue;

      // The key is the pair (current state, symbol)
      auto key = std::make_tuple(current_state, symbol_id);
      if (arc_map.find(key) != arc_map.end()){
        // If a transition with that symbol already exists, the existing transition is followed.
        current_state = arc_map[key];
      }
      else{
        // A new state is added and the transition is created.
        int new_state = fsa.AddState();
        fsa.AddArc(current_state, fst::StdArc(symbol_id, symbol_id, 0.0, new_state));
        arc_map[key] = new_state;
        current_state = new_state;
      }
    }
    // Mark the final state with weight 0.0 if it has not been marked already.
    if (fsa.Final(current_state) == fst::StdArc::Weight::Zero()){
      fsa.SetFinal(current_state, 0.0);
    }
  }

  void FastaFstBuilder::Save(const std::string &filename){
    // Create the output directory if it does not exist.
    size_t last_slash = filename.find_last_of("/");
    if (last_slash != std::string::npos){
      std::string dir = filename.substr(0, last_slash);
      mkdir(dir.c_str(), 0755);
    }

    // Save the symbol table to a separate file.
    std::string syms_filename = filename + ".syms";
    if (!syms.WriteText(syms_filename)){
      throw std::runtime_error("Failed to write symbol table to " + syms_filename);
    }

    // Minimize before saving
    fst::Minimize(&fsa);

    // Associate the symbol table with the FST (for both input and output) and write it to the file.
    fsa.SetInputSymbols(&syms);
    fsa.SetOutputSymbols(&syms);
    if (!fsa.Write(filename)){
      throw std::runtime_error("Failed to write FST to " + filename);
    }
  }
}
