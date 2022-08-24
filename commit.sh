#! /bin/bash

# make html
./reload_docs.sh

git add *
git commit -m "code documentation (still working...)"
git push repo main
