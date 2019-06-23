# Fush-Reload Covert Channel

## Build

```sh
mkdir build
cd build
cmake ..
make
```

You will find the sender & receiver executables in the '/build/bin' directory.

## Run
Make sure the sender & receiver share the same file and file offset.
Note: the shared file should not be empty.

### With default parameters
Sender:
```sh
./sender
```

Receiver:
```sh
./receiver
```

### Specifying the shared file and offset
Sender:
```sh
./sender -f shared.txt -o offset
```

Receiver:
```sh
./receiver -f shared.txt -o offset
```
