all:
	nodemon -e md,css --exec "make build"
build:
	pandoc REVIEW.md --variable urlcolor=green -V fontsize=11pt -V geometry:"margin=3cm" --strip-comments --wrap=preserve --katex -s -o "review.pdf" --toc-depth=6
