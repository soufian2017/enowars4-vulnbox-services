using Gamemaster.Models.Database;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Gamemaster.Models.View
{
#pragma warning disable CS8618
    public class UserView
    {
        public long Id { get; set; }
        public string Name { get; set; }
        public string Email { get; set; }
        public List<SessionView> Sessions { get; set; } = new List<SessionView>();
        public List<TokenStrippedView> Tokens { get; set; } = new List<TokenStrippedView>();
        public UserView (User u)
        {
            Id = u.Id;
            Name = u.Name;
            Email = u.Email;
            foreach (SessionUserLink sul in u.Sessions)
            {
                Sessions.Add(new SessionView(sul.Session));
            }
            foreach (Token t in u.Tokens)
            {
                Tokens.Add(new TokenStrippedView(t));
            }
        }
    }
#pragma warning restore CS8618
}