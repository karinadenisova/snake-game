package main

import (
	"bufio"
	"fmt"
	"math/rand"
	"os"
	"os/exec"
	"time"
)

type Point struct {
	X, Y int
}

var snake []Point
var food Point
var dir string
var score int
var width, height = 40, 20

func clear() {
	cmd := exec.Command("clear")
	cmd.Stdout = os.Stdout
	cmd.Run()
}

func initGame() {
	snake = []Point{{height / 2, width / 2}, {height / 2, width/2 - 1}, {height / 2, width/2 - 2}}
	dir = "RIGHT"
	score = 0
	rand.Seed(time.Now().UnixNano())
	food = Point{rand.Intn(height), rand.Intn(width)}
}

func draw() {
	clear()
	for i := 0; i < height+2; i++ {
		fmt.Print("#")
	}
	fmt.Println()
	for i := 0; i < height; i++ {
		for j := 0; j < width; j++ {
			if j == 0 {
				fmt.Print("#")
			}
			isSnake := false
			for _, s := range snake {
				if s.X == i && s.Y == j {
					isSnake = true
					break
				}
			}
			if isSnake {
				fmt.Print("O")
			} else if food.X == i && food.Y == j {
				fmt.Print("F")
			} else {
				fmt.Print(" ")
			}
			if j == width-1 {
				fmt.Print("#")
			}
		}
		fmt.Println()
	}
	for i := 0; i < height+2; i++ {
		fmt.Print("#")
	}
	fmt.Printf("\nScore: %d | Use WASD. Press q to quit.\n", score)
}

func readInput() {
	reader := bufio.NewReader(os.Stdin)
	ch, _, _ := reader.ReadRune()
	switch ch {
	case 'a', 'A':
		if dir != "RIGHT" {
			dir = "LEFT"
		}
	case 'd', 'D':
		if dir != "LEFT" {
			dir = "RIGHT"
		}
	case 'w', 'W':
		if dir != "DOWN" {
			dir = "UP"
		}
	case 's', 'S':
		if dir != "UP" {
			dir = "DOWN"
		}
	case 'q', 'Q':
		os.Exit(0)
	}
}

func update() {
	head := snake[0]
	var newHead Point
	switch dir {
	case "RIGHT":
		newHead = Point{head.X, head.Y + 1}
	case "LEFT":
		newHead = Point{head.X, head.Y - 1}
	case "UP":
		newHead = Point{head.X - 1, head.Y}
	case "DOWN":
		newHead = Point{head.X + 1, head.Y}
	}
	if newHead.X < 0 || newHead.X >= height || newHead.Y < 0 || newHead.Y >= width {
		fmt.Println("\nGAME OVER!")
		os.Exit(0)
	}
	for _, seg := range snake {
		if seg == newHead {
			fmt.Println("\nGAME OVER!")
			os.Exit(0)
		}
	}
	snake = append([]Point{newHead}, snake...)
	if newHead == food {
		score++
		food = Point{rand.Intn(height), rand.Intn(width)}
	} else {
		snake = snake[:len(snake)-1]
	}
}

func main() {
	initGame()
	go func() {
		for {
			readInput()
		}
	}()
	for {
		draw()
		update()
		time.Sleep(100 * time.Millisecond)
	}
}
