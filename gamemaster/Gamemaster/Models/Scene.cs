using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Gamemaster.Models
{
    public class Scene
    {
        public ConcurrentDictionary<string, Unit> Units { get; set; } = new ConcurrentDictionary<string, Unit>();

        internal void AddUnit(string id, Unit unit)
        {
            Units.TryAdd(id, unit);
        }
        internal void Drag(string id, int x, int y)
        {
            Units[id].X = x;
            Units[id].Y = y;
        }
        internal void Move(string id, Direction d)
        {
            switch (d)
            {
                case Direction.North:
                    Units[id].Y -= 1;
                    break;
                case Direction.East:
                    Units[id].X += 1;
                    break;
                case Direction.South:
                    Units[id].Y += 1;
                    break;
                case Direction.West:
                    Units[id].X -= 1;
                    break;
            }
        }

        internal void RemoveUnit(string id)
        {
            Units.TryRemove(id, out var _);
        }
    }
}
