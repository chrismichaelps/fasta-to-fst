#ifndef FASTA_FST_BUILDER_HPP
#define FASTA_FST_BUILDER_HPP

#include <fst/fstlib.h>
#include <string>
#include <unordered_map>
#include <tuple>
#include <functional>

namespace std
{
  template <>
  struct hash<std::tuple<int, int>>
  {
    size_t operator()(const std::tuple<int, int> &key) const
    {
      return std::hash<int>()(std::get<0>(key)) ^
             (std::hash<int>()(std::get<1>(key)) << 1);
    }
  };
}

namespace fst
{

  class FastaFstBuilder
  {
    // FST to build and the symbol table for amino acids.
    fst::VectorFst<fst::StdArc> fsa;
    fst::SymbolTable syms;
    // Map to avoid adding duplicate transitions: (current state, symbol) -> next state.
    std::unordered_map<std::tuple<int, int>, int> arc_map;

    void InitializeSymbolTable();
    int GetSymbolId(char c) const;

  public:
    FastaFstBuilder();
    void AddSequence(const std::string &sequence);
    void Save(const std::string &filename);
  };

} // namespace fst

#endif
