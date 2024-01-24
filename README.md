# trumpetsim

트럼펫 시뮬레이션

## Activating Enscripten

```bash
source /path/to/emsdk/emsdk_env.sh
```

## Building

```bash
mkdir build
cd build
emcmake cmake ..
emmake make
```

## Running

```bash
npx http-server -o ./build
```
