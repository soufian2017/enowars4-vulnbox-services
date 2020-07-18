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
using Npgsql;
using System.Threading.Tasks;
using Gamemaster.Models.View;
using System.Net.Sockets;

namespace Gamemaster.Database
{
    public partial interface IGamemasterDb
    {
        void Migrate();
    }

    public partial class GamemasterDb : IGamemasterDb
    {
        public static Random Rand = new Random(); 
        private const int MAX_PASSWORD_LENGTH = 128;
        private readonly ILogger Logger;
        private readonly GamemasterDbContext _context;
        private readonly byte[] pepper = Encoding.UTF8.GetBytes("topsecretpepper!");
        const int MIGRATION_ATTEMPTS = 100;
        public GamemasterDb(GamemasterDbContext context, ILogger<GamemasterDb> logger)
        {
            _context = context;
            Logger = logger;
            
        }
        public void Migrate()
        {
            for (int i = 0; i < MIGRATION_ATTEMPTS; i++)
            {
                try
                {
                    var pendingMigrations = _context.Database.GetPendingMigrations().Count();
                    if (pendingMigrations > 0)
                    {
                        Logger.LogInformation($"Applying {pendingMigrations} migration(s)");
                        _context.Database.Migrate();
                        _context.SaveChanges();
                        Logger.LogDebug($"Database migration complete");
                    }
                    else
                    {
                        Logger.LogDebug($"No pending migrations");
                        return;
                    }
                }
                catch (PostgresException e)
                {
                    Console.WriteLine($"Database migration failed: {e.Message}");
                }
                catch (SocketException e)
                {
                    Console.WriteLine($"Database connection failed: {e.Message}");
                }
                catch (NpgsqlException e)
                {
                    Console.WriteLine($"Database something failed: {e.Message}");
                }
                Task.Delay(1000).Wait();
            }
            throw new Exception("Database unavailable!");
        }
        private void Hash(string password, ReadOnlySpan<byte> salt, Span<byte> output)
        {
            if (password.Length > MAX_PASSWORD_LENGTH) return;
            Span<byte> input = stackalloc byte[salt.Length + MAX_PASSWORD_LENGTH+pepper.Length];
            salt.CopyTo(input);
            Encoding.UTF8.GetBytes(password, input.Slice(salt.Length));
            pepper.CopyTo(input.Slice(salt.Length + password.Length));
            using var sha512 = new SHA512Managed();
            sha512.TryComputeHash(input.Slice(0, salt.Length + password.Length+pepper.Length), output, out var _);
        }
    }
}
