package boxedwine.org;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Vector;

import static java.nio.file.StandardCopyOption.REPLACE_EXISTING;

public class Main {
    static String wineZip;
    static String scriptDir;
    static String boxedWineExe = "boxedwine";
    static Vector<String> extraCommands = new Vector<>();
    static boolean verbose = false;
    static boolean atleastOneFailed = false;

    static class Results {
        int exitCode;
        boolean scriptFinished;
        String commandLine;
        int timeToComplete;
        Vector<String> output;
    }
    static public void copyFolder(Path src, Path dest) throws IOException {
        Files.walk(src)
                .forEach(source -> copy(source, dest.resolve(src.relativize(source))));
    }

    static private void copy(Path source, Path dest) {
        try {
            Files.copy(source, dest, REPLACE_EXISTING);
        } catch (Exception e) {
            throw new RuntimeException(e.getMessage(), e);
        }
    }

    public static void runBoxedWine(String directory, String scriptDir, String[] parts, Results results) throws IOException {
        ProcessBuilder builder = new ProcessBuilder();
        List<String> commands = new ArrayList<>();
        commands.add(boxedWineExe);
        commands.add("-automation");
        commands.add(directory+File.separator+scriptDir);
        commands.add("-root");
        commands.add(directory+File.separator+"root");
        commands.add("-zip");
        commands.add(wineZip);
        commands.add("-w");
        commands.add(parts[0]);
        for (int i=2;i<parts.length;i++) {
            commands.add(parts[i]);
        }
        for (int i=0;i<extraCommands.size();i++) {
            commands.add(extraCommands.elementAt(i));
        }
        commands.add("/bin/wine");
        commands.add(parts[1]);
        builder.command(commands);
        results.commandLine = "boxedwine -automation "+directory+File.separator+scriptDir+" -root "+directory+File.separator+"root -zip \""+wineZip+"\" -w \""+parts[0]+"\" /bin/wine \""+parts[1]+"\"";
        for (int i=2;i<parts.length;i++) {
            results.commandLine+=" "+parts[i];
        }
        for (int i=0;i<extraCommands.size();i++) {
            results.commandLine+=" "+extraCommands.elementAt(i);
        }
        if (verbose) {
            System.out.println(results.commandLine);
        }
        builder.directory(new File(directory));
        Process process = builder.start();
        StreamGobbler streamGobbler = new StreamGobbler(process.getInputStream());
        streamGobbler.run();
        try {
            results.exitCode = process.waitFor();
            results.scriptFinished = streamGobbler.scriptFinished;
            results.output = streamGobbler.lines;
        } catch (InterruptedException e) {
            throw new IOException("Failed to run "+parts[1]);
        }
    }

    public static void deleteDir(File file) throws IOException {
        if (file.isDirectory()) {
            File[] entries = file.listFiles();
            if (entries != null) {
                for (File entry : entries) {
                    deleteDir(entry);
                }
            }
        }
        if (!file.delete()) {
            throw new IOException("Failed to delete " + file);
        }
    }

    public static String[] getLines(String path) {
        try {
            List lines = Files.readAllLines(Paths.get(path));
            String[] result = new String[lines.size()];
            for (int i=0;i<lines.size();i++) {
                result[i] = (String)lines.get(i);
            }
            return result;
        } catch (IOException e) {

        }
        return new String[0];
    }

    public static void runTest(String filesPath, String path, Results results) throws IOException {
        File rootPath = new File(path+File.separator+"root");
        if (rootPath.exists()) {
            deleteDir(rootPath);
        }
        rootPath.mkdir();
        File copiedFilesPath = new File(path+File.separator+"root"+File.separator+"files");
        copyFolder(new File(filesPath).toPath(), copiedFilesPath.toPath());

        String[] installLines = getLines(path + File.separator+"Install.txt");
        String[] installLines2 = getLines(path + File.separator+"Install2.txt");
        String[] playLines = getLines(path + File.separator+"Play.txt");

        long startTime = System.currentTimeMillis();
        if (installLines.length>0) {
            if (installLines.length<2) {
                throw new IOException(path+File.separator+"Install.txt needs to have 2 lines");
            }
            runBoxedWine(path, "install", installLines, results);
            if (results.exitCode!=1) {
                return;
            }
        }
        if (installLines2.length>0) {
            if (installLines2.length<2) {
                throw new IOException(path+File.separator+"Install2.txt needs to have 2 lines");
            }
            runBoxedWine(path, "install2", installLines2, results);
            if (results.exitCode!=1) {
                return;
            }
        }
        if (playLines.length>0) {
            if (playLines.length<2) {
                throw new IOException(path+File.separator+"Play.txt needs to have 2 lines");
            }
            runBoxedWine(path, "play", playLines, results);
        }
        results.timeToComplete = (int)((System.currentTimeMillis()-startTime)/1000);
        if (results.exitCode==1) {
            deleteDir(rootPath);
        }
    }

    public static void runTest(String name) {
        try {
            Results results = new Results();
            runTest(scriptDir+".."+File.separator+"files"+File.separator+name, scriptDir+name, results);
            if (results.exitCode==1) {
                System.out.println("OK   "+name+" completed in "+results.timeToComplete+" seconds");
            } else {
                System.out.println("FAILED "+name);
                atleastOneFailed = true;
                if (results.scriptFinished) {
                    System.out.println("    Script succeeded but Boxedwine did not exit cleanly");
                }
                System.out.println(results.commandLine);
                for (String line : results.output) {
                    System.out.println("    "+line);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        for (int i=0;i<args.length;i++) {
            System.out.println(args[i]);
        }
        if (args.length<2) {
            System.out.print("You must pass in <zip dir> and <script dir>");
            return;
        }
        int index=0;
        for (;index<args.length;index++) {
            if (args[index].equals("-v")) {
                verbose = true;
                continue;
            } else {
                break;
            }
        }
        wineZip = args[index++];
        scriptDir = args[index++];
        if (!scriptDir.endsWith("\\") && !scriptDir.endsWith("/")) {
            scriptDir+=File.separator;
        }
        if (args.length>1) {
            boxedWineExe = args[index++];
        }
        if (verbose) {
            System.out.println("zipDir = "+wineZip);
            System.out.println("scriptDir = "+scriptDir);
            System.out.println("boxedWineExe = "+boxedWineExe);
        }
        if (args.length>2) {
            for (;index<args.length;) {
                extraCommands.addElement(args[index++]);
            }
        }
        File dir = new File(scriptDir);
        File[] scripts = dir.listFiles();
        if (scripts==null) {
            System.out.println("Did not find any script folder in "+scriptDir);
            return;
        }
        Arrays.sort(scripts);
        long startTime = System.currentTimeMillis();
        for (File f : scripts) {
            if (f.isDirectory()) {
                runTest(f.getName());
            }
        }
        int elapsedTime = (int)((System.currentTimeMillis()-startTime)/1000);
        int minutes = elapsedTime / 60;
        int seconds = elapsedTime - minutes*60;
        System.out.println("Total Time: "+minutes+"m "+seconds+"s");
        if (atleastOneFailed) {
            System.exit(1);
        }
    }
}
