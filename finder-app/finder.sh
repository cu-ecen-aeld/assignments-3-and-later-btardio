filesdir=$1
searchstr=$2

if [ -z "$1" ]; then
	echo "First parameter required, please specify a file directory."
	return 1
fi

if [ -z "$2" ]; then
	echo "Second parameter required, please specify a search string."
	return 1
fi

numberfiles=find / 
