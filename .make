#!/bin/bash

main() {
	./.stop

	pushd ../..
	./.makeall; err=$?
	popd

	return $err	
}

main "$@"
