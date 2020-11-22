from ctf-18.04

user root
run apt-get update && apt-get install -y libclang-6.0-dev libgl1 libqt5x11extras5 libpython3.8
run ln -sf /usr/bin/python3 /usr/bin/python
run ln -sf /usr/bin/pip3 /usr/bin/pip
run ln -s /lib64/ld-linux-x86-64.so.2 /lib64/ld-lsb-x86-64.so.3

copy ./ida /home/kylebot/src/ida
copy ./git /home/kylebot/src/CSE576_Project/.git
copy ./ida.reg /home/kylebot/.idapro/ida.reg
copy ./code /home/kylebot/code
run chown -R kylebot:kylebot /home/kylebot/
run pip install jsonlines

user kylebot
run cd /home/kylebot/src/CSE576_Project && git checkout . && pip install -e .
run sed -i 's/\/usr\/local\/lib\//\/usr\/lib\/llvm-6.0\/lib\//g' /home/kylebot/src/CSE576_Project/placeholder/tokenizers/c_tokenizer.py

entrypoint bash -c "cd ~/src/ida/flexlm && ./run.sh" && /bin/bash
