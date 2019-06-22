# Fush-Reload Covert Channel

## Compile

```sh
make -f Makefile_sender
make -f Makefile_receiver
```

## Run

Make sure the sender & receiver share the same file and file offset

Sender:
```sh
./sender -f shared.txt -o offset
```

Receiver:
```sh
./receiver -f shared.txt -o offset
```
