#!/bin/bash
git ls-files | grep -v '^data/' | grep -v '^images/' | xargs wc -l
