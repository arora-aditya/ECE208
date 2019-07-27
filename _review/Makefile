all:
	nodemon -e md,css --exec "make build"
build:
	pandoc REVIEW.md --variable urlcolor=green -V geometry:"top=2cm, bottom=2cm, left=1cm, right=1cm" --strip-comments --wrap=preserve --katex -s -o "review.pdf" --toc-depth=6
