# Fasta to FST

This project demonstrates how to convert FASTA files to Finite State Transducers (FSTs). It provides a simple workflow for building the project, converting a FASTA file to an FST, and visualizing the result.


## Follow these steps to install OpenFST on macOS M1.

## 1. Download OpenFST
- Download the latest OpenFST release from [OpenFST Releases](http://www.openfst.org/twiki/bin/view/FST/FstDownload).
- In this example, I used `openfst-1.8.2`.

## 2. Prepare for Installation
- Move the downloaded `openfst-1.8.2` folder to your desired directory.

## 3. Install OpenFST
1. Copy the `install_openfst.sh` script into the `openfst-1.8.2` directory.
```sh
chmod +x install_openfst.sh
./install_openfst.sh
```

## Rebuild the Project

```bash
make clean && make
```

## Convert FASTA to FST

Run the program again:

```bash
bin/fasta2fst data/input/small_example.fasta data/output/example.fst
```

## Print the FST

To inspect the content of the generated FST, use the following command. This will print the FST using the provided symbol table:

```bash
fstprint --isymbols=data/output/example.fst.syms --osymbols=data/output/example.fst.syms data/output/example.fst
```

## Visualize the FST

You can generate a PDF visualization of the FST using fstdraw and graphviz. This will create a visual representation of the FST:

```bash
fstdraw --isymbols=data/output/example.fst.syms --osymbols=data/output/example.fst.syms data/output/example.fst | \
    dot -Tpdf > data/output/example.pdf
open data/output/example.pdf
```

...

## **:handshake: Contributing**

- Fork it!
- Create your feature branch: `git checkout -b my-new-feature`
- Commit your changes: `git commit -am 'Add some feature'`
- Push to the branch: `git push origin my-new-feature`
- Submit a pull request

---

### **:busts_in_silhouette: Credits**

- [Chris Michael](https://github.com/chrismichaelps) (Project Leader, and Developer)

---

### **:anger: Troubleshootings**

This is just a personal project created for study / demonstration purpose and to simplify my working life, it may or may
not be a good fit for your project(s).

---

### **:heart: Show your support**

Please :star: this repository if you like it or this project helped you!\
Feel free to open issues or submit pull-requests to help me improving my work.

<a href="https://www.buymeacoffee.com/chrismichael" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-red.png" alt="Buy Me A Coffee" style="height: 60px !important;width: 217px !important;" ></a>

---

### **:robot: Author**

_*Chris M. Perez*_

> You can follow me on
> [github](https://github.com/chrismichaelps)&nbsp;&middot;&nbsp;[twitter](https://twitter.com/Chris5855M)

---

Copyright ©2025 [FAST to FST](https://github.com/chrismichaelps/fasta-to-fst).
