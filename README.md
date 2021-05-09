# Montecarlo pi

A parallel implementation of the Montecarlo method known as dartboard algorithm to approximate pi (π).

## Installation

Install gcc compiler:

```bash
sudo apt install gcc
```

## Usage

- Execute the bash script to run the tests:

  ```bash
  bash run.sh
  ```

- To execute a single file, `cd` into `bin/` and run any of the following commands:

  ```bash
  ./dart_seq {exponent}
  ./dart_th {exponent} {threads}
  ./dart_th_off {exponent} {threads}
  ```

  Replace `{exponent}` with a number from 0 to 11 and `{threads}` with the number of threads to use. For example, if you pass `7`, then the program will raise 10 to the power of 7, which will calculate 10,000,000 throws.
