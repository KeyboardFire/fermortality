#!/bin/bash

cd bin/debug/linux
cp /usr/local/lib/libglfw.so.3 .
cat <<'EOS' > Fermortality.sh
#!/bin/bash
LD_PRELOAD="$(readlink -m libglfw.so.3)" ./Fermortality
EOS
chmod +x Fermortality.sh
cd ..
cp -r linux Fermortality
tar czf Fermortality.tar.gz Fermortality
rm -r Fermortality
mv Fermortality.tar.gz ../..
