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
emcc --pre-js pre.js --post-js post.js --bind -o oniguruma.html onig-string.cpp onig-reg-exp.cpp onig-result.cpp onig-searcher.cpp onig-scanner.cpp -O3 -s WASM=1 --shell-file html_template/amd.html -Ideps/onig -L/usr/local/lib -lonig
```