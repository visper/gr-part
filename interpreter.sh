#!/bin/bash

echo creating dot file

#rm *.ps #delete previous solution

dot -Tps graph.gv -o graph.ps

echo lost time "$SECONDS"sec

exit 1
