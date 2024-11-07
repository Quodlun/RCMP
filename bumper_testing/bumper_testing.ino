#define bumperPin 17

void setup ()
{
  pinMode ( bumperPin, OUTPUT );
  digitalWrite ( bumperPin, LOW );
}

void loop ()
{
  digitalWrite ( bumperPin, HIGH );
  delay ( 5000 );

  digitalWrite ( bumperPin, LOW );
  delay ( 5000 );
}