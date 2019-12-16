using System;
using System.Threading.Tasks;
using Discord.WebSocket;
using Discord.Commands;
using Microsoft.Extensions.DependencyInjection;
using System.Reflection;
using Discord;

namespace feeldopebot
{
  
    class Program
    {
        // static void Main(string[] args) => new Program().RunBotAsync().GetAwaiter().GetResult();
      
        public DiscordSocketClient CClient;
        public CommandService CComands;
        public IServiceProvider CServices;

        public async Task RunBotAsync()
        {
            CClient = new DiscordSocketClient();
            CComands = new CommandService();
            CServices = new ServiceCollection()
                .AddSingleton(CClient)
                .AddSingleton(CComands)
                .BuildServiceProvider();

            string bot_token = "NTkyODQyMjY5NDMwOTA2ODgw.XRFN5Q.6DI0FBEZG7aLQggVKdbqm4JvL50";

            CClient.Log += Log;

            await RegisterCommandsAsync();

            await CClient.LoginAsync(Discord.TokenType.Bot, bot_token);

            await CClient.StartAsync();

            await CClient.SetGameAsync("feel-dope", "", ActivityType.Listening);

            await Task.Delay(-1);
        }

        private Task Log(LogMessage arg)
        {
            Console.WriteLine(arg);
            return Task.CompletedTask;
        }
       
        public async Task RegisterCommandsAsync()
        {
            CClient.MessageReceived += HandleCommandAsync;

            await CComands.AddModulesAsync(Assembly.GetEntryAssembly());
        }

        private async Task HandleCommandAsync(SocketMessage arg)
        {
            var message = arg as SocketUserMessage;

            if (message is null || message.Author.IsBot) return;

            int argPos = 0;

            if (!(message.HasMentionPrefix(CClient.CurrentUser, ref argPos) || message.HasCharPrefix('$', ref argPos))) return;

            var context = new CommandContext(CClient, message);

            var result = await CComands.ExecuteAsync(context, argPos, CServices);

            if (!result.IsSuccess)
                Console.WriteLine(result.ErrorReason);

        }
        public static void Run() => new Program().RunBotAsync().GetAwaiter().GetResult();
        static void Main(string[] args)
        {
            Console.Title = "feel-dope logs";
            Run();
        }

    }
}
