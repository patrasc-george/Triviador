# Triviador

The Triviador application is a strategy quiz game that involves multiple players competing to control regions on a map by answering trivia questions. The game's primary objective is to gain control over as many regions as possible, accumulate points, and ultimately rank higher than other players by the end of the game. The gameplay is divided into several stages, including base selection, region conquest, and duels.

In the initial setup, the game reads trivia questions from a file and initializes the game environment, including the map and the players. Each player logs in with a username and password. The map is created with a specific number of rows and columns based on the number of players participating, ensuring a balanced game environment. Players then proceed to the base selection stage, where each player selects a region to establish their base. This selection is determined by answering a numerical trivia question, with the closest answer to the correct one gaining the privilege of choosing first.

Following the base selection, the game enters the region conquest stage. Players take turns selecting regions to expand their territory, again determined by answering numerical trivia questions. Each player can select multiple regions based on their ranking, and the process continues until all regions on the map are owned.

The duel stage is the core of the game, where players attack neighboring regions owned by other players. The attacking player and the defending player face off in a duel by answering multiple-choice trivia questions. The player who answers correctly, or more accurately and quickly, wins the duel. If the attacker wins, they can decrement the score of the defending region and potentially conquer it if the score reaches zero. Conquering a base leads to the attacker gaining control of all the defender's territories. The game continues through several rounds of duels until a set number of rounds is reached or only one player remains with territories.

The application also includes an advantage system, where players can use accumulated points to gain advantages during duels, such as eliminating wrong answers or receiving hints. The advantage usage is strategically important and can turn the tide in critical moments of the game.

At the end of the game, a ranking system determines the final standings based on the total scores accumulated by the players. The player with the highest score is declared the winner. The application provides detailed feedback and displays the final map, showing the distribution of territories among players.

The Triviador game is designed to be interactive and engaging, combining strategic territorial control with the challenge of trivia questions. It encourages players to think quickly and strategically to outsmart their opponents and secure victory. The game's modular design allows for easy updates and the addition of new features, ensuring a dynamic and enjoyable gaming experience.
