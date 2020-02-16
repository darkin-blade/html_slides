if [ "$1"x = x ]
then
  echo "\033[1;31mneed message\033[0m"
else
  echo "\033[1;32mAdding...\033[0m"
  git add -A
  git commit -m $1
  echo "\033[1;33mPushing...\033[0m"
  if [ "$2"x != x ]
  then
    git push origin master
  else
    echo "\033[1;35mAbort\033[0m"
  fi
fi
