# snake-term
Snake game in terminal written in c. Featuring adaptive resizing and colored text graphics!

![](/images/snake.gif)

## Dependencies
The only dependency is ncurses, install on debian/ubuntu via 
```
  $ sudo apt-get install libncurses5-dev libncursesw5-dev
```

## Building the Project
Once the project is cloned into a directory, cd into that directory, and run the following commands:
```
  $ make
```

## Running the Project
To run with default configuration:
```
  $ ./snake
```
To configure the dimensions of the play area
```
  $ ./snake -d [width]x[height]
```
Any other options given will just print the help information
