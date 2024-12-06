#include <Discord_WebHook.h>
#include "ExternVariable.h"
#include "PinMap.h"

Discord_Webhook discord;

void setup ()
{
  Serial.begin ( 115200 );

  discord.begin ( DISCORD_WEBHOOK ); // Initialize the Discord_Webhook object
  discord.disableDebug (); // Disable debug (no serial message will be send)
  discord.addWiFi ( ssid, password ); // Add WiFi credentials (you can add multiples WiFi SSID)
  discord.connectWiFi (); // Connect to WiFi

  bool message_sent = discord.send ( "Hello World" ); // Send message
  if ( message_sent )
  {
    Serial.println("Message sent");
  }
  
  else
  {
    Serial.println("I AM ERROR");
  }

  discord.send ( "Other Test" );
}

void loop () {}