exit_with_usage() {
    echo "./utility.sh [help | build | run] <args...>\n\tbuild: mode=[dbg|rel]\n" && exit $1
}

build() {
    if [[ $1 = "dbg" ]]; then
        make clean && make all EXTRA_FLAGS="-g -Og"
    elif [[ $1 = "rel" ]]; then
        make clean && make all EXTRA_FLAGS="-O2"
    else
        echo "Invalid option for configure_build!" && exit 1
    fi
}

action="$1"

if [[ $action = "help" ]]; then
    exit_with_usage 0
elif [[ $action = "build" ]]; then
    build "$2"
elif [[ $action = "run" ]]; then
    make run
else
    exit_with_usage 1
fi
