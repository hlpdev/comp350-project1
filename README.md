# comp350-project1

### Building

```
git clone https://github.com/hlpdev/comp350-project1.git
cd comp350-project1
```

requires `gcc`
```bash
chmod +x ./build.sh

./build.sh
```

### Usage

```bash
chmod +x ./run.sh

# run single process:
./run [start] [end]

# run multi-process:
./run [start] [end] [proc_count]
```

### Cleanup

```bash
chmod +x ./clean.sh

./clean.sh
```

### Issues

Since we use return codes for the result of each process, if there are more than 255 primes in a single batch, then the result will overflow and the result will be incorrect.
