#!/bin/bash
#
# Deploy script to Amazon EC2 storage.
# Removes development files for a "clean" copy
export AWS_DEFAULT_PROFILE=ardusat

# Print usage
function usage() {
  echo -n "$(basename $0) [OPTIONS] [FILE]...
  Deploys a new version of the SDK live to AWS. This makes updates to the
  CHANGELOG and increments the version.

Options:
  -v, --version-string   Specify a new version string for this version
  -z, --zip-only	 Only package zip, don't upload to S3
  -n, --no-version       Don't update the version string
  -h, --help             Display this help and exit
"
}

function deploy () {
    cd ../
    mkdir tmp_ArdusatSDK-Logging
    cp -r ./ArdusatSDK-Logging tmp_ArdusatSDK-Logging/ArdusatSDK-Logging
    cd tmp_ArdusatSDK-Logging
    rm -rf ./ArdusatSDK-Logging/.git ./ArdusatSDK-Logging/decode_binary ./ArdusatSDK-Logging/.ycm* ./ArdusatSDK-Logging/*.pyc ./ArdusatSDK-Logging/deploy_sdk.sh ./ArdusatSDK-Logging/.gitignore
    zip -r ArdusatSDK-Logging.zip ./ArdusatSDK-Logging
    cp -f ArdusatSDK-Logging.zip ~/Downloads/ArdusatSDK-Logging.zip
}

function upload () {
    aws s3 cp ./ArdusatSDK-Logging.zip s3://ardusatweb/ArdusatSDK-Logging.zip
}

function clean () {
    cd ../
    rm -rf ./tmp_ArdusatSDK-Logging
}

no_update=0
upload=1
while getopts "hznv:" opt; do
    case $opt in
	h)
	    usage
	    exit 0
	    ;;
	v)
	    version_string=$OPTARG
	    ;;
	n)
	    no_update=1
	    ;;
	z)
	    no_upload=0
	    ;;
	\?)
	    echo "Invalid option -$OPTARG"
	    usage
	    exit 1
	    ;;
    esac
done
shift $((OPTIND-1))

if [[ $no_update -eq 0 ]]; then
  if [ -z $version_string ]; then
      default_str=`awk '/##\ \[.*\] -/ { print substr($2, 2, length($2) - 2); exit; }' CHANGELOG.md`
      new_minor_version=$(($(echo $default_str | sed 's/.*\.\([0-9][0-9]*\)$/\1/') + 1))
      default_str=$(echo $default_str | sed 's/\(.*\)\.[0-9][0-9]*$/\1/').$new_minor_version
      echo "Enter a version string ($default_str):"
      read version_string
      if [ -z $version_string ]; then
	  version_string=$default_str
      fi
  fi

  echo "Enter a CHANGELOG description for SDK version $version_string"
  FILE=$(mktemp -t $(basename $0));
  vim "$FILE";
  changelog=`cat $FILE`
  rm "$FILE"

  echo "CHANGELOG description:"
  echo "$changelog"

  awk -v v="## [$version_string] - $(date +%Y-%m-%d)" -v d="$changelog\n" 'NR == 4 { print v; print d; } { print }' CHANGELOG.md > CHANGELOG.md.new

  mv CHANGELOG.md.new CHANGELOG.md
fi

deploy
if [[ $no_upload -eq 1 ]]; then
  upload
fi
clean
