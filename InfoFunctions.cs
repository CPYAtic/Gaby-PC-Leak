using Discord;
using Discord.Commands;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
namespace feeldopebot.Modules
{
    public class InfoFunctions : ModuleBase<CommandContext>
    {
        [Command("showallusers")]
        public async Task MassKickAsync(string reason = null)
        {
            IGuild guild = Context.Guild;
            var builder = new EmbedBuilder();
            builder.WithTitle("All users info");

            foreach (var user in guild.GetUsersAsync().Result)
                builder.AddField(user.Username + " - Creation date: ", user.CreatedAt);

            builder.WithColor(Color.DarkPurple);
            await Context.Channel.SendMessageAsync("", false, builder.Build());
            await Task.CompletedTask;
        }

        [Command("serverinfo")]
        public async Task GetServerInfo()
        {
            IGuild guild = Context.Guild;
            var users = await guild.GetUsersAsync();
            var builder = new EmbedBuilder();
            builder.WithTitle(Context.Guild.Name + "'s serverinfo");
            builder.AddField("Creation date: ", Context.Guild.CreatedAt);
            builder.AddField("Owner of server: ", "<@" + Context.Guild.OwnerId + ">");
            builder.AddField("Members count: ", users.Count);
            //builder.WithThumbnailUrl("url");
            builder.WithColor(Color.DarkPurple);
            await Context.Channel.SendMessageAsync("", false, builder.Build());
        }

        [Command("help")]
        public async Task GetHelpCommands()
        {
            IGuild guild = Context.Guild;
            var users = await guild.GetUsersAsync();
            var builder = new EmbedBuilder();
            builder.WithTitle("feel-dope's commands");

            builder.AddField("$help - ", "Shows feel-dope's commands");
            builder.AddField("$serverinfo - ", "Shows current serverinfo");
            builder.AddField("$showallusers - ", "Shows all users in server and their creation date");
            builder.AddField("$checkwhitelist - ", "Gets highest role position of an user");
            builder.AddField("$masstextchannel [name] [ammount] - ", "Mass creates text channels");
            builder.AddField("$massvoicechannel [name] [ammount] - ", "Mass creates voice channels");
            builder.AddField("$massdm [message] - ", "Mass dm'es everyone");
            builder.AddField("$massnick [nickname] - ", "Changes everyone's nickname");
            builder.AddField("$massban - ", "Mass bans everyone");
            builder.AddField("$masskick - ", "Mass kicks everyone");


            builder.WithColor(Color.DarkPurple);
            await Context.Channel.SendMessageAsync("", false, builder.Build());
        }

    }
}
