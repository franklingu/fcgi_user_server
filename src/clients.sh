echo "Starting simulating clients..."
for i in `seq 1 1000`; do
    ./simple_tcpclient 5432 &
done
wait
