package main

import (
    "fmt"
    "log"

    "github.com/bwmarrin/discordgo"
)

// Replace this with your bot token
const token = "YOUR_BOT_TOKEN"

func main() {
    // Create a new Discord session using the provided bot token
    dg, err := discordgo.New("Bot " + token)
    if err != nil {
        fmt.Println("error creating Discord session,", err)
        return
    }

    // Register the messageCreate function as a callback for MessageCreate events
    dg.AddMessageCreate(messageCreate)

    // Open a websocket connection to Discord and begin listening.
    err = dg.Open()
    if err != nil {
        fmt.Println("error opening connection,", err)
        return
    }

    fmt.Println("Bot is now running. Press CTRL+C to exit.")
    select {}
}

// This function will be called when the bot receives a message
func messageCreate(s *discordgo.Session, m *discordgo.MessageCreate) {
    // Ignore messages from the bot itself
    if m.Author.ID == s.State.User.ID {
        return
    }

    if m.Content == "!hello" {
        s.ChannelMessageSend(m.ChannelID, "Hello, "+m.Author.Username+"!")
    }
}
