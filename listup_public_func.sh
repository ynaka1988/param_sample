#!/bin/bash -u

function listup_public_func()
{
	local dir=${1}

	pushd ${dir} > /dev/null

	find . -name "*\.c" | xargs -l -I{} global {} -f --color | grep -v "static" | grep -v "#define"

	popd > /dev/null
}

function check_tag_exist()
{
	local dir=${1}
	local ret=1

	if test -f ${dir}/GTAGS; then
		gtags ${dir}
		ret=${?}
	else
		echo "tag file not exist"
		echo "Do I create GTAGS ? [y/n]"
		read ans

		case ${ans} in
			"" | [Yy]* )
				gtags ${dir}
				ret=${?}
				;;
			* )
				;;
		esac
	fi

	return ${ret}
}

function check_gtags()
{
	if type gtags > /dev/null 2>&1; then
		return 0
	else
		echo "gtags not installed"
		echo "Please install gnu global"
		echo "# sudo apt-get install global"
		return 1
	fi
}


function main()
{
	if [ $# != 1 ];then
		echo "argument num miss-match"
		return 1
	fi

	local dir=${PWD}/${1}

	check_gtags && check_tag_exist ${dir} && listup_public_func ${dir}
}

main ${@}

