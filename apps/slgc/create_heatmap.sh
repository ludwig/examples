#!/bin/bash -v
python slgc_process_data.py
matlab -nodisplay -nojvm -r slgc_create_image
#scp seven.png server-ip:public_html/
