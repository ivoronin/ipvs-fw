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

### keepalived.conf
```
...
virtual_server 192.168.0.1 80 {
    lvs_sched fw
    ...
    ! Primary server
    real_server 192.168.0.2 80 {
        weight 101
        ...
    }
    ! Standby server
    real_server 192.168.0.3 80 {
        weight 100
        ...
    }
    ...
}
...
```
