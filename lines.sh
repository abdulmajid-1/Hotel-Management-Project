#!/bin/bash
git ls-files | grep -v '^data/' | xargs wc -l
