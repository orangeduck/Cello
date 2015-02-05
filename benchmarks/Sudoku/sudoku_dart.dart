import 'dart:core';
import 'dart:io';
import 'dart:typeddata';

class Sudoku {
  final _R = new List<Uint16List>(324);
  final _C = new List<List<int>>(729);
  
  Sudoku() {
    final Uint8List nr = new Uint8List(324);
    for (int i = 0, r = 0; i < 9; ++i)
      for (int j = 0; j < 9; ++j)
        for (int k = 0; k < 9; ++k)
          _C[r++] = <int>[ 9 * i + j, (i~/3*3 + j~/3) * 9 + k + 81, 9 * i + k + 162, 9 * j + k + 243 ];
    for (int c = 0; c < 324; ++c) {
      _R[c] = new Uint16List(9);
      nr[c] = 0;
    }
    for (int r = 0; r < 729; ++r)
      for (int c2 = 0; c2 < 4; ++c2) {
        int k = _C[r][c2];
        _R[k][nr[k]++] = r;
      }
  }
  _update(sr, sc, r, v) {
    int min = 10, min_c = 0;
    for (int c2 = 0; c2 < 4; ++c2) sc[_C[r][c2]] += v<<7;
    for (int c2 = 0; c2 < 4; ++c2) {
      int r2, rr, cc2, c = _C[r][c2];
      if (v > 0) {
        for (r2 = 0; r2 < 9; ++r2) {
          if (sr[rr = _R[c][r2]]++ != 0) continue;
          for (cc2 = 0; cc2 < 4; ++cc2) {
            var cc = _C[rr][cc2];
            if (--sc[cc] < min) { min = sc[cc]; min_c = cc; }
          }
        }
      } else { // revert
        for (r2 = 0; r2 < 9; ++r2) {
          if (--sr[rr = _R[c][r2]] != 0) continue;
          var p = _C[rr];
          ++sc[p[0]]; ++sc[p[1]]; ++sc[p[2]]; ++sc[p[3]];
        }
      }
    }
    return min<<16 | min_c; // return the col that has been modified and with the minimal available choices
  }
  solve(s_) {
    int j, r, c, r2, min, hints = 0;
    var sr = new Int16List(729), sc = new Int16List(324);
    var cr = new Int16List(81), cc = new Int16List(81), out = new Int16List(81);
    List<String> ret = [];
    for (r = 0; r < 729; ++r) sr[r] = 0;
    for (c = 0; c < 324; ++c) sc[c] = 9;
    for (int i = 0; i < 81; ++i) {
      int a = s_.codeUnitAt(i) >= 49 && s_.codeUnitAt(i) <= 57? s_.codeUnitAt(i) - 49 : -1;
      if (a >= 0) _update(sr, sc, i * 9 + a, 1);
      if (a >= 0) ++hints;
      cr[i] = cc[i] = -1; out[i] = a + 1;
    }

    for (int i = 0, dir = 1, cand = 10<<16|0;;) {
      while (i >= 0 && i < 81 - hints) {
        if (dir == 1) {
          min = cand>>16; cc[i] = cand&0xffff;
          if (min > 1) {
            for (c = 0; c < 324; ++c) {
              if (sc[c] < min) {
                min = sc[c]; cc[i] = c;
                if (min <= 1) break;
              }
            }
          }
          if (min == 0 || min == 10) cr[i--] = dir = -1;
        }
        c = cc[i];
        if (dir == -1 && cr[i] >= 0) _update(sr, sc, _R[c][cr[i]], -1);
        for (r2 = cr[i] + 1; r2 < 9; ++r2)
          if (sr[_R[c][r2]] == 0) break;
        if (r2 < 9) {
          cand = _update(sr, sc, _R[c][r2], 1);
          cr[i++] = r2; dir = 1;
        } else cr[i--] = dir = -1;
      }
  
      if (i < 0) break;

      Uint8List y = new Uint8List(81);
      for (j = 0; j < 81; ++j) y[j] = out[j];
      for (j = 0; j < i; ++j) { r = _R[cc[j]][cr[j]]; y[r~/9] = r%9 + 1; }
      ret.add(y.join());
      --i; dir = -1;
    }
    return ret;
  }
}

main()
{
  final List<String> argv = new Options().arguments;
  final fp = new File(argv[0]);
  final lines = fp.readAsLinesSync(encoding: Encoding.ASCII);
  final Sudoku s = new Sudoku();
  for (int i = 0; i < lines.length; ++i) {
    var rst = s.solve(lines[i]);
    for (final x in rst) print(x);
    print("");
  }
}
