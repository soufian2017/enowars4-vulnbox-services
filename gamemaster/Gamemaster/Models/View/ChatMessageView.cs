using Gamemaster.Models.Database;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Gamemaster.Models.View
{
#pragma warning disable CS8618
    public class ChatMessageView
    {
        public long Id { get; set; }
        public string SenderName { get; set; }
        public long SessionContextId { get; set; }
        public string Content { get; set; }
        public DateTime Timestamp { get; set; }
        public ChatMessageView (ChatMessage m)
        {
            Id = m.Id;
            SenderName = m.Sender.Name;
            SessionContextId = m.SessionContextId;
            Content = m.Content;
            Timestamp = m.Timestamp;
        }
    }
#pragma warning restore CS8618
}