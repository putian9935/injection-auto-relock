hostname=rp-f05f88.local

# try if RP is connected
echo Attempting to find device $hostname...
ip4=$(ping -c 1 $hostname)
if [ "$?" -eq 0 ]; then
    echo Device found!
else
    echo Cannot find device $hostname
    exit -1
fi

# find IP
ip4=$(echo $ip4 | head -n1 | awk '{print $3}')
ip4=${ip4:1:-1}
echo "IP address for device $hostname is $ip4"

# upload
scp -r ./src/ root@$ip4:~/injection_stabilizer/
