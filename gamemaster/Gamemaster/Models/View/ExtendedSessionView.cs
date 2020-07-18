using Gamemaster.Models.Database;
using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace Gamemaster.Models.View
{
    public class ExtendedSessionView
    {
        public long Id { get; set; }
        public string OwnerName { get; set; }
        public DateTime Timestamp { get; set; }
        public string Name { get; set; }
        public string Notes { get; set; }
        public ExtendedSessionView(Session s)
        {
            Name = s.Name;
            Timestamp = s.Timestamp;
            OwnerName = s.Owner.Name;
            Id = s.Id;
            Notes = s.Notes;
        }
    }
}
