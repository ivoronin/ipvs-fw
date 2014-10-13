# ip_vs_fw
Fixed weighted scheduler for IP Virtual Server - the highest weight
non-overloaded destination is only used when other destination(s) are given
lower weight values.

If there are several destinations with the highest weight, the first one is used.

## Requirements
Linux 3.10.0

## Installation
```
make
make modules_install
```
