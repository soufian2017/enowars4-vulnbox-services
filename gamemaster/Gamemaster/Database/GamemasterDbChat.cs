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
        Task<ChatMessage> InsertChatMessage(Session context, User sender, string content);
        Task<ChatMessageView[]> GetChatMessages(long context);
    }
    public partial class GamemasterDb : IGamemasterDb
    {
        public async Task<ChatMessage> InsertChatMessage(Session context, User sender, string content)
        {
            var msg = new ChatMessage()
            {
                Sender = sender,
                SessionContext = context,
                Content = content,
                Timestamp = DateTime.UtcNow,

            };
            _context.ChatMessages.Add(msg);
            await _context.SaveChangesAsync();
            return msg;
        }
        public async Task<ChatMessageView[]> GetChatMessages(long context)
        {
            return await _context.ChatMessages
                .Where(msg => msg.SessionContextId == context)
                .Include(msg => msg.Sender)
                .OrderBy(msg => msg.Id)
                .Take(100)
                .Select(m => new ChatMessageView(m))
                .ToArrayAsync();
        }
    }
}
