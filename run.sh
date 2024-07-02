if [[ $(grep -i Microsoft /proc/version) ]]; then
    (cd out/build/; ./PittyPutty > /dev/null 2>&1) # to mute errors related to audio (no audio if executed from wsl)
else
    (cd out/build/; ./PittyPutty)
fi