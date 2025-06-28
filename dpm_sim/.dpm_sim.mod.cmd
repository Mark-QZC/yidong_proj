savedcmd_/home/gyy/dpm_sim/dpm_sim.mod := printf '%s\n'   dpm_sim.o | awk '!x[$$0]++ { print("/home/gyy/dpm_sim/"$$0) }' > /home/gyy/dpm_sim/dpm_sim.mod
