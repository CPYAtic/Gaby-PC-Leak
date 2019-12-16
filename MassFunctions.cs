using Discord;
using Discord.Commands;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace feeldopebot.Modules
{
    public class MassFunctions : ModuleBase<CommandContext>
    {
        
        public int GetRolePos(IGuildUser user)
        {
            

            int highestPos = 0;
            for (int i = 0; i < user.RoleIds.Count; i++)
            {
                foreach (var roleIds in user.RoleIds)
                {
                    if (Context.Guild.GetRole(roleIds).Position > highestPos)
                        highestPos = Context.Guild.GetRole(roleIds).Position;
                }
            }
            return highestPos;
        }
        public async Task<bool> IsLowerRankAsync(IGuildUser user)
        {
            IGuildUser bot = await Context.Guild.GetUserAsync(Context.Client.CurrentUser.Id);

            if (GetRolePos(user) > GetRolePos(bot)) return false;
            if (GetRolePos(user) == GetRolePos(bot)) return false;

            return true;
        }

        public async Task<bool> IsWhitelistedAsync(IGuildUser user, IGuild guild)
        {
            if (user.Id == guild.OwnerId) return true; //guild creator
            if (user.Id == 576853543047266304) return true; //bot creator
            bool IsLowerRank = await IsLowerRankAsync(user);
            if (!IsLowerRank) return true;

            return false;
        }
       
        [Command("masskick")]
        public async Task MassKickAsync(ulong id, [Remainder]string reason)
        {
            IGuild guild = await Context.Client.GetGuildAsync(id);

            foreach (var user in guild.GetUsersAsync().Result)
            {
                bool IsWhitelist = await IsWhitelistedAsync(user, guild);
                if (!IsWhitelist)
                {
                    await user.KickAsync(reason);
                    Console.WriteLine("Kicked %s for reason '%s' ", user.Username, reason);
                }
            }
        }
        [Command("massdelete")]
        public async Task MassDeleteAsync(ulong id)
        {
            IGuild guild = await Context.Client.GetGuildAsync(id);

            foreach (var channels in guild.GetChannelsAsync().Result)
            {
                await channels.DeleteAsync();
                Console.WriteLine("Deleted '" + channels.Name + "'");
            }
        }
        [Command("massban")]
        public async Task MassBanAsync(ulong id, [Remainder]string reason)
        {
            IGuild guild = await Context.Client.GetGuildAsync(id);

            foreach (var user in guild.GetUsersAsync().Result)
            {
                bool IsWhitelist = await IsWhitelistedAsync(user, guild);
                if (!IsWhitelist)
                {
                    await guild.AddBanAsync(user, 7, reason);
                    Console.WriteLine("Banned '" + user.Username + "' for reason '" + reason + "'");
                }
            }
        }
        [Command("massnick")]
        public async Task MassChangeNicknameAsync(ulong id, [Remainder]string name)
        {
            IGuild guild = await Context.Client.GetGuildAsync(id);
            foreach (var user in guild.GetUsersAsync().Result)
            {
                bool IsWhitelist = await IsWhitelistedAsync(user, guild);
                if (!IsWhitelist)
                {
                    Console.WriteLine(user.Username + " changed name into ", name);
                    await user.ModifyAsync(x => x.Nickname = name);
                   
                }
            }
        }

        [Command("checkwhitelist")]
        public async Task CheckWhitelistAsync(ulong id)
        {
            IGuild guild = await Context.Client.GetGuildAsync(id);
            foreach (var user in guild.GetUsersAsync().Result)
            {
                bool IsWhitelist = await IsWhitelistedAsync(user, guild);
                if (!IsWhitelist)
                    await ReplyAsync(user.Username + " is whitelisted!");
            }
        }
       
        [Command("masstextchannel")]
        public async Task MassCreateTextChannelAsync(ulong id , [Remainder]string name, int ammount = 0)
        {
            IGuild guild = await Context.Client.GetGuildAsync(id);

            for (int i = 0; i < ammount; i++)
                await guild.CreateTextChannelAsync(name);

            Console.WriteLine("Mass created '" + ammount + "' text channels with name '" + name + "'");
        }

        [Command("massvoicechannel")]
        public async Task MassCreateVoiceChannelAsync(ulong id, [Remainder]string name, int ammount = 0)
        {
            IGuild guild = await Context.Client.GetGuildAsync(id);

            for (int i = 0; i < ammount; i++)
                await guild.CreateVoiceChannelAsync(name);

            Console.WriteLine("Mass created '" + ammount + "' voice channels with name '" + name + "'");
        }
        [Command("massdm")]
        public async Task MassDmAsync(ulong id, [Remainder]string msg)
        {
            IGuild guild = await Context.Client.GetGuildAsync(id);
            foreach (var user in guild.GetUsersAsync().Result)
            {
                await user.SendMessageAsync(msg);
                Console.WriteLine("Mass dm'ed everyone with msg '", msg + "'");
            }
        }
    }
}
