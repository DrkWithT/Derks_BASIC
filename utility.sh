exit_with_usage() {
    echo "./utility.sh [help | build | run] <args...>\n\tbuild: mode=[debug|release]\n" && exit $1
}

build() {
    if [[ $1 != "debug" && $1 != "release" ]]; then
        echo "Invalid option for configure_build!" && exit 1;
    fi

    [ $1 = "debug" ] && cmake --fresh -S . -B build -DDO_DEBUG_MODE:BOOL=1 && cmake --build build || cmake --fresh -S . -B build -DDO_DEBUG_MODE:BOOL=0 && cmake --build build

    [ $? -eq 0 ] && mv ./build/compile_commands.json .
}

action="$1"

if [[ $action = "help" ]]; then
    exit_with_usage 0
elif [[ $action = "build" ]]; then
    build "$2"
elif [[ $action = "run" ]]; then
    ./build/src/derkbasic $2
else
    exit_with_usage 1
fi
