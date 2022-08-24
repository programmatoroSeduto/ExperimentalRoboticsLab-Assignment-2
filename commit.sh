#! /bin/bash

# make html
./reload_docs.sh

git add *
# git commit -m "code documentation (still working...)"
# git commit -m "code documentation (still working...)"
git commit -m "code documentation (ready for the RCL#2 readme)"
git push repo main
