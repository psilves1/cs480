    # Fush-Reload Covert Channel

## Build

```
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
```
./sender
```
and 

Receiver:
```
./receiver
```

### Specifying the shared file and offset
Sender:
```
./sender -f shared.txt -o offset
```

and 


Receiver:
```
./receiver -f shared.txt -o offset
```
