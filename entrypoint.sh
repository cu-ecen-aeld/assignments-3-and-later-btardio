#!/bin/bash
/actions-runner/config.sh --name myrunner --unattended --replace --url https://github.com/cu-ecen-aeld/assignment-1-btardio --token AFAF6AOJ6GPQTGDHCE2LZSLHRMXDM
/actions-runner/run.sh&
exec "$@"
