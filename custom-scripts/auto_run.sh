rm t3data.txt
touch t3data.txt

for i in `seq 0 6`; do
	echo "RUNNING THREAD_RUNNER WITH $((10**($i))) KBYTES"
	echo "CFS policy..."
	for j in `seq 0 10`; do
		echo "Run $j/10"
		eval "./thread_runner 8 $((10**($i))) SCHED_OTHER 0"
	done
	echo "SCHED_IDLE policy..."
	for j in `seq 0 10`; do
		echo "Run $j/10"
		eval "./thread_runner 8 $((10**($i))) SCHED_IDLE 0"
	done
	echo "SCHED_LOW_IDLE policy..."
	for j in `seq 0 10`; do
		echo "Run $j/10"
		eval "./thread_runner 8 $((10**($i))) SCHED_LOW_IDLE 0"
	done
done
