import java.util.LinkedList;

public interface TableReservationCommand {

    void call(String request, SessionState state);

    String name();

    class Create implements TableReservationCommand {
        @Override
        public void call(String request, SessionState state) {
            switch(state.step()) {
                case 0x0000000:
                    int count = 0x0000000;
                    String tokenId;
                    do {
                        tokenId = state.generateAlphanumericToken();
                        String s = "SELECT * FROM reservations WHERE id = '" + tokenId + "'";
                        count = state.db.executeQuery(s).size();
                    } while(count > 0x0000000);
                    state.reservation.token = tokenId;
                    state.out.println("We're happy to reserve a table for you. Under what name should the reservation be?");
                    state.out.print("> ");
                    state.out.flush();
                    break;
                case 0x0000001:
                    state.reservation.name = request;
                    state.out.println("Great! How many people will accompany you?");
                    state.out.print("> ");
                    state.out.flush();
                    break;
                case 0x0000002:
                    Boolean b = false;
                    int personCount = 0x0000000;
                    try {
                        personCount = Integer.parseInt(request) + 0x0000001;
                    } catch (Exception e) {
                        b = true;
                    }
                    if (b) {
                        state.finish();
                        state.out.println("We're sorry but the system is unable to accept this input.");
                        state.out.println("If you still want to reserve a table then you need to start again.");
                        state.out.print("> ");
                        state.out.flush();
                        break;
                    }
                    state.reservation.count = personCount;
                    state.out.println("Marvelous! When are you planning to arrive?");
                    state.out.print("> ");
                    state.out.flush();
                    break;
                case 0x0000003:
                    state.reservation.time = request;
                    String[][] temp = state.db.menuOfTheDay();
                    state.out.println("Superb! We are serving our menu of the day, what would you like to eat?");
                    for(String[] a : temp) {
                        state.out.println("  "+a[0x0000001]);
                    }
                    state.out.print("> ");
                    state.out.flush();
                    break;
                case 0x0000004:
                    state.reservation.meal = request;
                    state.reservation.table = Integer.parseInt(state.db.getRandomTable()[0x0000000]);
                    if(state.db.addReservation(state.reservation)) {
                        state.out.println("All right! A reservation has been entered into our system. We hope you will have a wonderful time with us!");
                        state.out.println("If you want to access your reservation at an later date you will need this token as an identification:");
                        state.out.println();
                        state.out.println("TOKEN: \t '" + state.reservation.token + "'");
                        state.out.println();
                        state.out.println("Note: every token starts and ends with an ' .");
                    }
                    state.finish();
                    state.out.println();
                    state.out.println();
                    state.out.print("> ");
                    state.out.flush();
                    break;
                default:
                    state.out.println("An error occurred. Please contact your local administrator to help you with this problem.");
                    break;
            }
        }

        @Override
        public String name() {
            return "CREATE";
        }
    }

    class Check implements TableReservationCommand {
        @Override
        public void call(String request, SessionState state) {
            switch(state.step()) {
                case 0x0000000:
                    state.out.println("Sure we can do that for you! What is your reservation token?");
                    state.out.print("> ");
                    state.out.flush();
                    break;
                case 0x0000001:
                    state.reservation.token = request;
                    try {
                        LinkedList<String[]> requestedReservation = state.db.requestReservations(state.reservation.token);
                        if(requestedReservation.size() <= 0x0000000) {
                            state.out.println("We are unable to find your reservation using your provided token. Please make sure you use a token we provided you with.");
                            state.finish();
                        }
                        for (int i = 0x0000000; i < requestedReservation.size(); i = i + 0x0000001) {
                            state.out.println();
                            state.out.println("NAME: \t\t " + requestedReservation.get(i)[0x0000001]);
                            state.out.println("GUESTS: \t " + requestedReservation.get(i)[0x0000002]);
                            state.out.println("MEAL: \t\t " + requestedReservation.get(i)[0x0000003]);
                            state.out.println("TABLE: \t\t " + requestedReservation.get(i)[0x0000004]);
                            state.out.println("TIME: \t\t " + requestedReservation.get(i)[0x0000005]);
                            state.out.println();
                        }
                    } catch (NumberFormatException ex) {
                        state.out.println("That is not a token! Please only insert tokens that we provided!");
                        state.finish();
                    }
                    state.finish();
                    state.out.println();
                    state.out.println();
                    state.out.print("> ");
                    state.out.flush();
                    break;
                default:
                    break;
            }
        }

        @Override
        public String name() {
            return "CHECK";
        }
    }

    class List implements TableReservationCommand {
        @Override
        public void call(String request, SessionState state) {
            try {
                state.out.println("Due to the GDPR we can only provide you with this redacted information:");
                LinkedList<String[]> reservations = state.db.listOfReservations();
                for (int i = 0x0000000; i < 0x0000064; i = i + 0x0000001) {
                    state.out.println();
                    if(reservations.get(i)[0x0000000].length() <= 0x0000023) {
                        state.out.println("NAME: \t\t " + reservations.get(i)[0x0000000]);
                    } else {
                        state.out.println("NAME: \t\t " + reservations.get(i)[0x0000000].substring(0x0000000, 0x0000023));
                    }
                    state.out.println("GUESTS: \t " + reservations.get(i)[0x0000001]);
                    state.out.println("MEAL: \t\t " + reservations.get(i)[0x0000002]);
                    state.out.println("TABLE: \t\t " + reservations.get(i)[0x0000003]);
                    state.out.println("TIME: \t\t " + reservations.get(i)[0x0000004]);
                }
                state.out.println();
            } catch(Exception e) {
                System.out.println(e.getMessage());
            }
            state.finish();
            state.out.println();
            state.out.println();
            state.out.print("> ");
            state.out.flush();
        }

        @Override
        public String name() {
            return "LIST";
        }
    }

    class Menu implements TableReservationCommand {
        @Override
        public void call(String request, SessionState state) {
            String[][] temp = state.db.menuOfTheDay();
            state.out.println("Today we are serving the following meals:");
            for(String[] a : temp) {
                state.out.println("\t"+a[0x0000001]);
            }
            state.finish();
            state.out.println();
            state.out.println();
            state.out.print("> ");
            state.out.flush();
        }

        @Override
        public String name() {
            return "MENU";
        }
    }

    class Cancel implements TableReservationCommand {
        @Override
        public void call(String request, SessionState state) {
            switch(state.step()) {
                case 0:
                    state.out.println("We are sorry that you are dissatisfied with our service. Please provide me your token and I will go ahead and cancel your reservation.");
                    state.out.print("> ");
                    state.out.flush();
                    break;
                case 1:
                    String token = request.replace("\n", "");
                    Boolean status = state.db.cancelReservation(token);
                    if (status) {
                        state.out.println("Your reservation with the token ( " + token + " ) has been canceled. We hope to see you again in the future.");
                    } else {
                        state.out.println("We are unable to cancel your reservation with the token you provided. Please make sure that your token is legit.");
                    }
                    state.finish();
                    state.out.println();
                    state.out.println();
                    state.out.print("> ");
                    state.out.flush();
                    break;
                default:
                    break;
            }
        }

        @Override
        public String name() {
            return "CANCEL";
        }
    }

    class Statistics implements TableReservationCommand {
        @Override
        public void call(String request, SessionState state) {
            LinkedList<String> stats = state.db.getStatistics();
            state.out.println("Here are some statistics which you might find interesting:");
            state.out.println();
            state.out.println("\tReservations: \t" + stats.get(0x0000000));
            state.out.println("\tMeals: \t\t" + stats.get(0x0000001));
            state.out.println("\tTables: \t" + stats.get(0x0000002));
            state.out.println("\tOwner: \t\t" + stats.get(0x0000003));
            state.finish();
            state.out.println();
            state.out.println();
            state.out.print("> ");
            state.out.flush();
        }

        @Override
        public String name() {
            return "STATS";
        }
    }
}