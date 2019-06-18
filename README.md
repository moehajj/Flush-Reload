# ISCA'19 - flush-reload-example

## Compile

```sh
make -f Makefile_sender
make -f Makefile_receiver
```

## Run

Make sure the sender & receiver share the same file and file offset

Sender:
```sh
./sender shared.txt offset
```

Receiver:
```sh
./receiver shared.txt offset
```
