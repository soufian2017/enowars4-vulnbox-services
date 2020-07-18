using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Logging;
using Gamemaster.Models.Database;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;
using Gamemaster.Models.View;

namespace Gamemaster.Database
{
    public partial interface IGamemasterDb
    {
        Task<TokenStrippedView> GetTokenByUUID(string UUID);
        Task<TokenData> GetTokenDataByUUID(string UUID);
        Task<TokenStrippedView[]> GetTokens(long userid);
        Task<Token?> AddTokenToUser(long sessionId, string name, string description, bool isprivate, byte[] icon);
    }
    public partial class GamemasterDb : IGamemasterDb
    {
        public async Task<TokenStrippedView?> GetTokenByUUID(string UUID)
        {
            try
            {
                return new TokenStrippedView(await _context.Tokens.Where(t => t.UUID == UUID).Include(t => t.Owner).SingleOrDefaultAsync());
            }
            catch (Exception e)
            {
                Logger.LogError(e.ToString());
                return null;
            }
        }
        public async Task<TokenData> GetTokenDataByUUID(string UUID)
        {
            return new TokenData(await _context.Tokens.Where(t => t.UUID == UUID).SingleOrDefaultAsync());
        }
        public async Task<TokenStrippedView[]> GetTokens(long userid)
        {
            return await _context.Tokens.Where(t => t.OwnerId == userid).Select(t => new TokenStrippedView(t)).ToArrayAsync();
        }
        public async Task<Token?> AddTokenToUser(long userid, string name, string description, bool isprivate, byte[] icon)
        {
            string uUID = "";
            lock (Rand) for (; uUID.Length < 512; uUID += Rand.Next().ToString("X8")) ;
            var token = new Token()
            {
                Name = name,
                Description = description,
                IsPrivate = isprivate,
                Icon = icon,
                UUID = uUID,
                OwnerId = userid
            };
            _context.Tokens.Add(token);
            await _context.SaveChangesAsync();
            return token;
        }
    }
}
