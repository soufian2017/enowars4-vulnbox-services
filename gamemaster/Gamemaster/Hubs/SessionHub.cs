using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.SignalR;
using Microsoft.Extensions.Logging;
using Gamemaster.Database;
using Gamemaster.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Claims;
using System.Threading;
using System.Threading.Tasks;
using Gamemaster.Models.Database;
using Gamemaster.Models.View;
using Microsoft.Extensions.DependencyInjection;
using System.Collections.Concurrent;

namespace Gamemaster.Hubs
{
    [Authorize]
    public class SessionHub : Hub
    {
        public static ConcurrentDictionary<long, Scene> Scenes = new ConcurrentDictionary<long, Scene>();
        private readonly ILogger Logger;
        private readonly IServiceProvider ServiceProvider; //IGamemasterDb
        public static ConcurrentDictionary<string, long> ConIdtoSessionId = new ConcurrentDictionary<string, long>();
        public SessionHub(ILogger<SessionHub> logger, IServiceProvider serviceProvider)
        {
            Logger = logger;
            ServiceProvider = serviceProvider;
        }

        public override Task OnConnectedAsync()
        {
            Logger.LogInformation($"OnConnectedAsync NameIdentifier={Context.User.FindFirst(ClaimTypes.NameIdentifier)?.Value}, ConnectionId={Context.ConnectionId}, Scenes in Dict:{Scenes.Count()}, ConIDs in Dict: {ConIdtoSessionId.Count()}");
            return Task.CompletedTask;
        }

        public override async Task OnDisconnectedAsync(Exception exception)
        {
            Logger.LogInformation($"OnDisconnectedAsync NameIdentifier={Context.User.FindFirst(ClaimTypes.NameIdentifier)?.Value}, ConnectionId={Context.ConnectionId}, exception={exception}");
            ConIdtoSessionId.TryRemove(Context.ConnectionId, out var sceneId);
            Scenes.TryGetValue(sceneId, out var scene);
            if (scene != null)
            {
                scene.RemoveUnit("unit"+Context.ConnectionId);
                lock(scene)
                    if (scene.Units.Count() <= 0) Scenes.TryRemove(sceneId, out var _);
                await Clients.Group(sceneId.ToString()).SendAsync(nameof(scene), scene, CancellationToken.None);
            }
        }
        public async Task Chat(string Message)
        {
            using var scope = ServiceProvider.CreateScope();
            var db = scope.ServiceProvider.GetRequiredService<IGamemasterDb>();
            var currentUsername = Context.User.Identity.Name;
            if (currentUsername == null) return;
            var currentUser = (await db.GetUser(currentUsername));
            if (currentUser == null) return;
            var currentUserId = currentUser.Id;
            var sid = ConIdtoSessionId[Context.ConnectionId];
            var session = await db.GetFullSession(sid, currentUserId);
            if (session == null) return;
            var msg = await db.InsertChatMessage(session, currentUser, Message);
            var messages = await db.GetChatMessages(sid);
            await Clients.All.SendAsync("Chat", messages, Context.ConnectionAborted);
        }

        public async Task Join(long sid)
        {
            try
            {
                using var scope = ServiceProvider.CreateScope();
                var db = scope.ServiceProvider.GetRequiredService<IGamemasterDb>();
                var currentUsername = Context.User.Identity.Name;
                if (currentUsername == null)
                {
                    Logger.LogError($"{Context.ConnectionId} no name claim in session");
                    return;
                }
                var currentUser = (await db.GetUser(currentUsername));
                if (currentUser == null)
                {
                    Logger.LogError($"{Context.ConnectionId} {currentUsername} not in db");
                    return;
                }
                var currentUserId = currentUser.Id;
                var session = await db.GetSession(sid, currentUserId);
                if (session == null)
                {
                    Logger.LogError($"{Context.ConnectionId} session {sid} not in db");
                    return;
                }
                await Groups.AddToGroupAsync(Context.ConnectionId, sid.ToString());
                var scene = Scenes.GetOrAdd(sid, new Scene());
                ConIdtoSessionId.TryAdd(Context.ConnectionId, sid);
                SceneView sceneView;
                lock (scene)
                {
                    scene.AddUnit("unit" + Context.ConnectionId, new Unit());
                    sceneView = new SceneView(scene);
                }
                var messages = await db.GetChatMessages(sid);
                await Clients.Caller.SendAsync("Chat", messages, Context.ConnectionAborted);
                await Clients.Group(sid.ToString()).SendAsync("Scene", sceneView, Context.ConnectionAborted);
                Logger.LogInformation($"{Context.ConnectionId} join successfull");
            }
            catch(Exception e)
            {
                Logger.LogError($"{Context.ConnectionId} failed to join: {e.Message}\n{e.StackTrace}");
            }
        }
        public async Task Move(Direction d)
        {
            using var scope = ServiceProvider.CreateScope();
            var db = scope.ServiceProvider.GetRequiredService<IGamemasterDb>();
            var currentUsername = Context.User.Identity.Name;
            if (currentUsername == null) return;
            var currentUser = await db.GetUser(currentUsername);
            if (currentUser == null) return;
            var currentUserId = currentUser.Id;
            Logger.LogInformation($"Move, ID:::{Context.ConnectionId}");
            var sid = ConIdtoSessionId[Context.ConnectionId];
            var session = await db.GetSession(sid, currentUserId);
            if (session == null) return;
            Scenes[sid].Move("unit" + Context.ConnectionId, d);
            await Clients.Group(sid.ToString()).SendAsync("Scene", Scenes[sid], Context.ConnectionAborted);
        }
        public async Task Drag(int x, int y)
        {
            using var scope = ServiceProvider.CreateScope();
            var db = scope.ServiceProvider.GetRequiredService<IGamemasterDb>();
            var currentUsername = Context.User.Identity.Name;
            if (currentUsername == null) return;
            var currentUser = await db.GetUser(currentUsername);
            if (currentUser == null) return;
            var currentUserId = currentUser.Id;
            Logger.LogInformation($"Move, ID:::{Context.ConnectionId}");
            var sid = ConIdtoSessionId[Context.ConnectionId];
            var session = await db.GetSession(sid, currentUserId);
            if (session == null) return;
            Scenes[sid].Drag("unit" + Context.ConnectionId, x, y);
            await Clients.Group(sid.ToString()).SendAsync("Scene", Scenes[sid], Context.ConnectionAborted);
        }
    }
}
