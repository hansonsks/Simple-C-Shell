# Simple-C-Shell

A shell in C that mimics some of the most popular commands used in Bash but for Windows

The aliases implemented are intended to be paths, so entering `add_alias my_program C:\Users\Username\Downloads\my_program.exe`, 
then entering `launch my_program` will launch the program

This also works with PDFs / word docs (`open`) and folders (`explore`)

You may also add git commands as aliases, so entering `add_alias gpom "git push origin main"`, then add -> commit -> `gpom`,
the files will be pushed to GitHub.

As another example, you can enter `add_alias gcommit "git commit -m"`, add the files to be committed, and type `gcommit "Test Commit"`
to commit the files

Check `help` to see the commands that I implemented
