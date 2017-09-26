## Build
```bash
# Compile oniguruma
cd deps/onig
autoreconf -vfi
emconfigure ./configure
emmake make
emmake make install
cd -
# Compile the library and code together to HTML
emcc -o hello2.html hello2.cpp -O3 -s WASM=1 --shell-file html_template/shell_minimal.html -Ideps/onig -L/usr/local/lib -lonig
```